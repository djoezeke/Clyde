#ifndef CLYDE_SOUND_H
#define CLYDE_SOUND_H

#include "clyde.h"

#include <filesystem>
#include <vector>

namespace clyde
{

    namespace audio
    {

        /**
         * @brief Class for streaming audio data
         */
        class Stream
        {
        public:
            enum class Status
            {
                Paused,  //!< Sound Paused
                Stopped, //!< Sound Stopped
                Playing, //!< Sound Playing
            };

            virtual void pause();
            virtual void play();
            virtual void stop();

            [[nodiscard]] Status getStatus() const;

            void setPlayingOffset(Time timeOffset);
            [[nodiscard]] Time getPlayingOffset() const;

            struct Impl;
            std::unique_ptr<Impl> m_impl;
        };

        class Wave
        {
        };

    } // namespace audio

    /**
     * @brief Buffer holding decoded audio samples.
     */
    class SoundBuffer
    {
    public:
        SoundBuffer();
        ~SoundBuffer();

        explicit SoundBuffer(const std::filesystem::path &filename);
        SoundBuffer(const void *data, std::size_t sizeInBytes);

        /**
         * @brief Get the duration of the audio.
         * @return Duration as Time object
         */
        [[nodiscard]] Time getDuration() const;

        /**
         * @brief Get the sample rate of audio.
         * @return Sample rate in Hz
         */
        [[nodiscard]] int getSampleRate() const;

        /**
         * @brief Get number of audio channels.
         * @return Number of channels (1=mono, 2=stereo, etc.)
         */
        [[nodiscard]] int getChannelCount() const;

    private:
        friend class Sound;
        friend class Music;

        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    /**
     * @brief One-shot sound effect (loaded from buffer).
     */
    class Sound : public audio::Stream
    {
    public:
        explicit Sound(const SoundBuffer &buffer);
        virtual ~Sound();

        void setBuffer(const SoundBuffer &buffer);

        /**
         * @brief Play the sound from the buffer.
         */
        void play() override;

        /**
         * @brief Stop the sound.
         */
        void stop() override;

    private:
        friend class SoundBuffer;

        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    /**
     * @brief Streamed music (typically for longer audio).
     */
    class Music : public audio::Stream
    {
    public:
        Music();
        ~Music();
        
        explicit Music(const std::filesystem::path &filename);
        Music(const void *data, std::size_t sizeInBytes);

        /**
         * @brief Get the duration of the music.
         * @return Duration as Time object
         */
        [[nodiscard]] Time getDuration() const;

        /**
         * @brief Get the sample rate of music.
         * @return Sample rate in Hz
         */
        [[nodiscard]] int getSampleRate() const;

        /**
         * @brief Get number of audio channels.
         * @return Number of channels (1=mono, 2=stereo, etc.)
         */
        [[nodiscard]] int getChannelCount() const;

        /**
         * @brief Play the music stream.
         */
        void play() override;

        /**
         * @brief Stop the music stream.
         */
        void stop() override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

} // namespace clyde

#endif // CLYDE_SOUND_H
