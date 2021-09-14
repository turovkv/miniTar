#pragma once

#include <iosfwd>
#include <cstdint>
#include <climits>

namespace BitStream {
    static_assert(CHAR_BIT == 8);

    class oBitStream {
    public:
        explicit oBitStream(std::ostream &os);
        void write(uint8_t *src, std::size_t size);
        oBitStream &operator<<(bool bit);
        void clear();
        void reopen();
        void close();
        explicit operator bool() const;
        ~oBitStream();

    private:
        std::ostream &os_;
        uint8_t pos_;
        uint8_t buf_;
    };

    class iBitStream {
    public:
        explicit iBitStream(std::istream &is);
        void read(uint8_t *src, std::size_t size);
        iBitStream &operator>>(bool &bit);
        void clear();
        void reopen();
        void close();
        explicit operator bool() const;

    private:
        std::istream &is_;
        uint8_t pos_;
        uint8_t buf_;
    };

    class iBitStream_iterator {
    public:
        iBitStream_iterator();
        explicit iBitStream_iterator(iBitStream &is);
        bool operator*() const;
        iBitStream_iterator &operator++();
        bool operator==(const iBitStream_iterator &other) const;
        bool operator!=(const iBitStream_iterator &other) const;
        explicit operator bool() const;

    private:
        void iter_read();

    private:
        iBitStream *stream_;
        bool value_;
        bool is_ok_;
    };
}