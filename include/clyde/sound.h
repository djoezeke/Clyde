#ifndef CLYDE_SOUND_H
#define CLYDE_SOUND_H

#include "clyde.h"

namespace clyde
{

    namespace audio
    {

        class Stream
        {
        public:
        private:
            struct Impl;
            std::unique_ptr<Impl> m_impl;
        };

        class Buffer
        {
        public:
        private:
            struct Impl;
            std::unique_ptr<Impl> m_impl;
        };

    } // namespace audio

    class Wave
    {
    };

    class Sound
    {
    public:
    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    class Music
    {
    public:
    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

} // namespace clyde

#endif // CLYDE_SOUND_H