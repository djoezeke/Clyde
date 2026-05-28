#include "clyde/sound.h"
#include "clyde/clyde.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#pragma region minimp3

// Provide minimp3 implementation here
#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"

// Provide minimp3_ex implementation here
#define MINIMP3_EX_IMPLEMENTATION
#include "minimp3_ex.h"

#pragma endregion minimp3

#pragma region miniaudio

// Provide miniaudio implementation here
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#pragma endregion miniaudio

#pragma region audio_impl

namespace clyde
{

    using namespace std::chrono_literals;

    ////////////////////////////////////////////////////////////
    // Stream Impl
    ////////////////////////////////////////////////////////////

    struct audio::Stream::Impl
    {
        std::atomic<audio::Stream::Status> status{audio::Stream::Status::Stopped};
        Time playingOffset{};
        std::mutex mutex;
        std::condition_variable cv;
    };

    ////////////////////////////////////////////////////////////
    // SoundBuffer Impl
    ////////////////////////////////////////////////////////////

    struct SoundBuffer::Impl
    {
        clyde::Time duration{};
        int sampleRate{44100};
        int channels{2};
        std::vector<float> pcmData; ///< Raw PCM audio samples (interleaved)
        size_t currentFrame{0};     ///< Current frame position for streaming
    };

    ////////////////////////////////////////////////////////////
    // Sound Impl
    ////////////////////////////////////////////////////////////

    struct Sound::Impl
    {
        const SoundBuffer *buffer{nullptr};
        std::thread worker;
        std::atomic_bool running{false};
        size_t playbackFrame{0}; ///< Current playback position in PCM buffer
    };

    ////////////////////////////////////////////////////////////
    // Music Impl
    ////////////////////////////////////////////////////////////

    struct Music::Impl
    {
        clyde::Time duration{};
        int sampleRate{44100};
        int channels{2};
        std::vector<float> pcmData; ///< Raw PCM audio samples (interleaved)
        size_t playbackFrame{0};    ///< Current playback position
        std::thread worker;
        std::atomic_bool running{false};
    };

    SoundBuffer::~SoundBuffer() = default;
    Music::~Music() = default;
    ////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////

    static clyde::Time secondsToTime(double s)
    {
        clyde::Time t;
        t.m_microseconds = std::chrono::microseconds(static_cast<long long>(s * 1e6));
        return t;
    }

    static double timeToSeconds(const clyde::Time &t)
    {
        return static_cast<double>(t.m_microseconds.count()) / 1e6;
    }

    ////////////////////////////////////////////////////////////
    // Miniaudio Decoders
    ////////////////////////////////////////////////////////////

    /**
     * @brief Load and decode audio file using miniaudio decoders (WAV, FLAC, OGG).
     * @param path File path
     * @param outSampleRate Output sample rate
     * @param outChannels Output channel count
     * @param outPcmData Output PCM buffer (float, interleaved)
     * @return true on success
     */
    static bool decoderLoadAudio(const std::filesystem::path &path, int &outSampleRate,
                                 int &outChannels, std::vector<float> &outPcmData)
    {
        // Try miniaudio decoders (WAV, FLAC, OGG)
        ma_decoder decoder;
        ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 0, 0);

        if (ma_decoder_init_file(path.string().c_str(), &config, &decoder) != MA_SUCCESS)
        {
            return false;
        }

        outSampleRate = static_cast<int>(decoder.outputSampleRate);
        outChannels = static_cast<int>(decoder.outputChannels);

        // Read all frames
        const size_t framesToRead = 4096;
        std::vector<float> buffer(framesToRead * outChannels);
        size_t totalFrames = 0;

        while (true)
        {
            ma_uint64 framesRead = 0;
            ma_decoder_read_pcm_frames(&decoder, buffer.data(), framesToRead, &framesRead);
            if (framesRead == 0)
                break;

            outPcmData.insert(outPcmData.end(),
                              buffer.begin(),
                              buffer.begin() + (framesRead * outChannels));
            totalFrames += framesRead;
        }

        ma_decoder_uninit(&decoder);
        return totalFrames > 0;
    }

    /**
     * @brief Load and decode MP3 using minimp3.
     * @param path File path
     * @param outSampleRate Output sample rate
     * @param outChannels Output channel count
     * @param outPcmData Output PCM buffer (float, interleaved)
     * @return true on success
     */
    static bool decoderLoadMp3(const std::filesystem::path &path, int &outSampleRate,
                               int &outChannels, std::vector<float> &outPcmData)
    {
        mp3dec_file_info_t info;
        if (mp3dec_load(nullptr, path.string().c_str(), &info, nullptr, nullptr) != 0)
        {
            return false;
        }

        outSampleRate = info.hz;
        outChannels = info.channels;

        // Convert to float and store
        if (info.buffer && info.samples > 0)
        {
            outPcmData.resize(info.samples);
            for (size_t i = 0; i < info.samples; ++i)
            {
                outPcmData[i] = static_cast<float>(info.buffer[i]) / 32768.0f;
            }
            free(info.buffer);
            return true;
        }
        return false;
    }

    /**
     * @brief Load audio file with automatic format detection.
     * @param path File path
     * @param outSampleRate Output sample rate
     * @param outChannels Output channel count
     * @param outPcmData Output PCM buffer (float, interleaved)
     * @return true on success
     */
    static bool loadAudioFile(const std::filesystem::path &path, int &outSampleRate,
                              int &outChannels, std::vector<float> &outPcmData)
    {
        std::string ext = path.extension().string();
        // Convert to lowercase for comparison
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext == ".mp3")
        {
            return decoderLoadMp3(path, outSampleRate, outChannels, outPcmData);
        }
        else
        {
            // Use miniaudio for WAV, FLAC, OGG
            return decoderLoadAudio(path, outSampleRate, outChannels, outPcmData);
        }
    }

    ////////////////////////////////////////////////////////////
    // audio::Stream
    ////////////////////////////////////////////////////////////

    // Stream methods

    void audio::Stream::pause()
    {
        if (m_impl)
            m_impl->status = Status::Paused;
    }

    void audio::Stream::play()
    {
        if (m_impl)
            m_impl->status = Status::Playing;
    }

    void audio::Stream::stop()
    {
        if (m_impl)
        {
            m_impl->status = Status::Stopped;
            m_impl->playingOffset = Time();
        }
    }

    audio::Stream::Status audio::Stream::getStatus() const
    {
        if (!m_impl)
            return Status::Stopped;
        return m_impl->status.load();
    }

    void audio::Stream::setPlayingOffset(Time timeOffset)
    {
        if (m_impl)
            m_impl->playingOffset = timeOffset;
    }

    Time audio::Stream::getPlayingOffset() const
    {
        if (!m_impl)
            return Time();
        return m_impl->playingOffset;
    }

    ////////////////////////////////////////////////////////////
    // SoundBuffer
    ////////////////////////////////////////////////////////////

    SoundBuffer::SoundBuffer() : m_impl(std::make_unique<Impl>()) {}

    static bool loadWavHeader(const std::filesystem::path &path, int &outRate, int &outChannels, double &outSeconds, std::vector<float> *outPcm = nullptr)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
            return false;

        char riff[4];
        file.read(riff, 4);
        if (std::strncmp(riff, "RIFF", 4) != 0)
            return false;

        file.seekg(8);
        char wave[4];
        file.read(wave, 4);
        if (std::strncmp(wave, "WAVE", 4) != 0)
            return false;

        // Scan chunks for fmt and data
        bool foundFmt = false;
        bool foundData = false;
        uint16_t audioFormat = 0;
        uint16_t numChannels = 0;
        uint32_t sampleRate = 0;
        uint32_t byteRate = 0;
        uint16_t bitsPerSample = 0;
        uint32_t dataSize = 0;

        while (file)
        {
            char id[4];
            file.read(id, 4);
            uint32_t size = 0;
            file.read(reinterpret_cast<char *>(&size), 4);
            if (!file)
                break;

            if (std::strncmp(id, "fmt ", 4) == 0)
            {
                foundFmt = true;
                file.read(reinterpret_cast<char *>(&audioFormat), 2);
                file.read(reinterpret_cast<char *>(&numChannels), 2);
                file.read(reinterpret_cast<char *>(&sampleRate), 4);
                file.read(reinterpret_cast<char *>(&byteRate), 4);
                file.read(reinterpret_cast<char *>(&bitsPerSample), 2);
                // skip rest of fmt chunk
                if (size > 16)
                    file.seekg(size - 16, std::ios::cur);
            }
            else if (std::strncmp(id, "data", 4) == 0)
            {
                foundData = true;
                dataSize = size;
                // skip data
                file.seekg(size, std::ios::cur);
            }
            else
            {
                // skip unknown chunk
                file.seekg(size, std::ios::cur);
            }
        }

        if (!foundFmt || !foundData || byteRate == 0)
            return false;

        outRate = static_cast<int>(sampleRate);
        outChannels = static_cast<int>(numChannels);
        outSeconds = static_cast<double>(dataSize) / static_cast<double>(byteRate);
        return true;
    }

    SoundBuffer::SoundBuffer(const std::filesystem::path &filename) : m_impl(std::make_unique<Impl>())
    {
        int sr = 44100;
        int ch = 2;
        std::vector<float> pcm;

        // Try to load with full decoder
        if (loadAudioFile(filename, sr, ch, pcm))
        {
            m_impl->sampleRate = sr;
            m_impl->channels = ch;
            m_impl->pcmData = std::move(pcm);

            // Calculate duration from PCM data
            double samples = static_cast<double>(m_impl->pcmData.size()) / static_cast<double>(ch);
            double secs = samples / static_cast<double>(sr);
            m_impl->duration = secondsToTime(secs);
        }
        else
        {
            // Fallback: set defaults
            m_impl->sampleRate = 44100;
            m_impl->channels = 2;
            m_impl->duration = secondsToTime(1.0);
            m_impl->pcmData.clear();
        }
    }

    SoundBuffer::SoundBuffer(const void *data, std::size_t sizeInBytes) : m_impl(std::make_unique<Impl>())
    {
        // Not implemented: treat as empty buffer
        m_impl->sampleRate = 44100;
        m_impl->channels = 2;
        m_impl->duration = secondsToTime(0.0);
    }

    Time SoundBuffer::getDuration() const
    {
        return m_impl ? m_impl->duration : Time();
    }

    int SoundBuffer::getSampleRate() const
    {
        return m_impl ? m_impl->sampleRate : 0;
    }

    int SoundBuffer::getChannelCount() const
    {
        return m_impl ? m_impl->channels : 0;
    }

    ////////////////////////////////////////////////////////////
    // Sound
    ////////////////////////////////////////////////////////////

    Sound::Sound(const SoundBuffer &buffer) : m_impl(std::make_unique<Impl>())
    {
        static_cast<audio::Stream *>(this)->m_impl = std::make_unique<audio::Stream::Impl>();
        m_impl->buffer = &buffer;
    }

    Sound::~Sound()
    {
        if (m_impl)
        {
            m_impl->running = false;
            if (m_impl->worker.joinable())
                m_impl->worker.join();
        }
    }

    void Sound::setBuffer(const SoundBuffer &buffer)
    {
        if (m_impl)
            m_impl->buffer = &buffer;
    }

    void Sound::play()
    {
        if (!m_impl || !m_impl->buffer)
            return;

        if (!static_cast<audio::Stream *>(this)->m_impl)
            static_cast<audio::Stream *>(this)->m_impl = std::make_unique<audio::Stream::Impl>();

        if (m_impl->running)
            return;

        m_impl->running = true;
        m_impl->playbackFrame = 0;
        static_cast<audio::Stream *>(this)->m_impl->status = audio::Stream::Status::Playing;
        static_cast<audio::Stream *>(this)->m_impl->playingOffset = secondsToTime(0.0);

        // Launch playback thread that advances the playing offset in real-time
        m_impl->worker = std::thread([this]()
                                     {
            auto &s = *m_impl;
            const SoundBuffer *buf = s.buffer;
            if (!buf || buf->m_impl->pcmData.empty())
            {
                s.running = false;
                return;
            }

            int sr = buf->m_impl->sampleRate;
            int ch = buf->m_impl->channels;
            size_t totalFrames = buf->m_impl->pcmData.size() / ch;
            auto &streamImpl = *static_cast<audio::Stream*>(this)->m_impl;

            auto lastTime = std::chrono::steady_clock::now();
            while (s.running && streamImpl.status == audio::Stream::Status::Playing)
            {
                auto now = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed = now - lastTime;
                lastTime = now;

                // Advance playback position
                double elapsedSeconds = elapsed.count();
                s.playbackFrame = std::min(s.playbackFrame + static_cast<size_t>(elapsedSeconds * sr),
                                          totalFrames);
                double currentSeconds = static_cast<double>(s.playbackFrame) / sr;
                streamImpl.playingOffset = secondsToTime(currentSeconds);

                if (s.playbackFrame >= totalFrames)
                {
                    streamImpl.status = audio::Stream::Status::Stopped;
                    break;
                }

                std::this_thread::sleep_for(10ms);
            }
            s.running = false; });
    }

    void Sound::stop()
    {
        if (!m_impl)
            return;
        m_impl->running = false;
        if (m_impl->worker.joinable())
            m_impl->worker.join();
        if (static_cast<audio::Stream *>(this)->m_impl)
        {
            static_cast<audio::Stream *>(this)->m_impl->status = audio::Stream::Status::Stopped;
            static_cast<audio::Stream *>(this)->m_impl->playingOffset = Time();
        }
    }

    ////////////////////////////////////////////////////////////
    // Music
    ////////////////////////////////////////////////////////////

    Music::Music() : m_impl(std::make_unique<Impl>())
    {
        static_cast<audio::Stream *>(this)->m_impl = std::make_unique<audio::Stream::Impl>();
    }

    Music::Music(const std::filesystem::path &filename) : m_impl(std::make_unique<Impl>())
    {
        static_cast<audio::Stream *>(this)->m_impl = std::make_unique<audio::Stream::Impl>();

        int sr = 44100;
        int ch = 2;
        std::vector<float> pcm;

        // Load audio file with full decoder
        if (loadAudioFile(filename, sr, ch, pcm))
        {
            m_impl->sampleRate = sr;
            m_impl->channels = ch;
            m_impl->pcmData = std::move(pcm);

            // Calculate duration from PCM data
            double samples = static_cast<double>(m_impl->pcmData.size()) / static_cast<double>(ch);
            double secs = samples / static_cast<double>(sr);
            m_impl->duration = secondsToTime(secs);
        }
        else
        {
            m_impl->sampleRate = 44100;
            m_impl->channels = 2;
            m_impl->duration = secondsToTime(1.0);
            m_impl->pcmData.clear();
        }
    }

    Music::Music(const void *data, std::size_t sizeInBytes) : m_impl(std::make_unique<Impl>())
    {
        static_cast<audio::Stream *>(this)->m_impl = std::make_unique<audio::Stream::Impl>();
        m_impl->sampleRate = 44100;
        m_impl->channels = 2;
        m_impl->duration = secondsToTime(1.0);
    }

    Time Music::getDuration() const
    {
        return m_impl ? m_impl->duration : Time();
    }

    int Music::getSampleRate() const
    {
        return m_impl ? m_impl->sampleRate : 0;
    }

    int Music::getChannelCount() const
    {
        return m_impl ? m_impl->channels : 0;
    }

    void Music::play()
    {
        if (!m_impl)
            return;

        if (!static_cast<audio::Stream *>(this)->m_impl)
            static_cast<audio::Stream *>(this)->m_impl = std::make_unique<audio::Stream::Impl>();

        if (m_impl->running)
            return;

        m_impl->running = true;
        m_impl->playbackFrame = 0;
        static_cast<audio::Stream *>(this)->m_impl->status = audio::Stream::Status::Playing;
        static_cast<audio::Stream *>(this)->m_impl->playingOffset = secondsToTime(0.0);

        m_impl->worker = std::thread([this]()
                                     {
            auto &impl = *m_impl;
            if (impl.pcmData.empty())
            {
                impl.running = false;
                return;
            }

            int sr = impl.sampleRate;
            int ch = impl.channels;
            size_t totalFrames = impl.pcmData.size() / ch;
            auto &streamImpl = *static_cast<audio::Stream*>(this)->m_impl;
            auto lastTime = std::chrono::steady_clock::now();

            while (impl.running && streamImpl.status == audio::Stream::Status::Playing)
            {
                auto now = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed = now - lastTime;
                lastTime = now;

                // Advance playback position
                double elapsedSeconds = elapsed.count();
                impl.playbackFrame = std::min(impl.playbackFrame + static_cast<size_t>(elapsedSeconds * sr),
                                             totalFrames);
                double currentSeconds = static_cast<double>(impl.playbackFrame) / sr;
                streamImpl.playingOffset = secondsToTime(currentSeconds);

                if (impl.playbackFrame >= totalFrames)
                {
                    streamImpl.status = audio::Stream::Status::Stopped;
                    break;
                }

                std::this_thread::sleep_for(10ms);
            }
            impl.running = false; });
    }

    void Music::stop()
    {
        if (!m_impl)
            return;
        m_impl->running = false;
        if (m_impl->worker.joinable())
            m_impl->worker.join();
        if (static_cast<audio::Stream *>(this)->m_impl)
        {
            static_cast<audio::Stream *>(this)->m_impl->status = audio::Stream::Status::Stopped;
            static_cast<audio::Stream *>(this)->m_impl->playingOffset = Time();
        }
    }

} // namespace clyde

#pragma endregion audio_impl
