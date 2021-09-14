#include "BitStream.h"
#include <iostream>

namespace BitStream {
    oBitStream::oBitStream(std::ostream &os) :
            os_(os), pos_(0), buf_(0) {}

    void oBitStream::write(uint8_t *src, std::size_t size) {
        if (pos_ == 0) {
            for (; size > 0; --size, ++src) {
                os_.put(*src);
            }
        } else {
            for (; size > 0; --size, ++src) {
                for (uint8_t i = 0; i < CHAR_BIT; ++i) {
                    *this << (*src & (1u << i));
                }
            }
        }
    }

    oBitStream &oBitStream::operator<<(bool bit) {
        buf_ |= static_cast<uint8_t>(bit) << pos_;
        ++pos_;
        if (pos_ >= CHAR_BIT) {
            os_.put(buf_);
            pos_ = 0;
            buf_ = 0;
        }
        return *this;
    }

    void oBitStream::clear() {
        pos_ = 0;
        buf_ = 0;
    }

    void oBitStream::reopen() {
        close();
        os_.clear();
        os_.seekp(0, std::ios::beg);
    }

    void oBitStream::close() {
        if (pos_ != 0) {
            os_.put(buf_);
            clear();
        }
    }

    oBitStream::operator bool() const {
        return os_.operator bool();
    }

    oBitStream::~oBitStream() {
        close();
    }

    iBitStream::iBitStream(std::istream &is) :
            is_(is), pos_(0), buf_(is.get()) {}

    void iBitStream::read(uint8_t *src, std::size_t size) {
        if (pos_ == 0) {
            *src = buf_;
            --size; ++src;
            for (; size > 0; --size, ++src) {
                *src = is_.get();
            }
            buf_ = is_.get();
        } else {
            for (; size > 0; --size, ++src) {
                for (uint8_t i = 0; i < CHAR_BIT; ++i) {
                    bool bit = false;
                    *this >> bit;
                    *src |= static_cast<uint8_t>(bit) << i;
                }
            }
        }
    }

    iBitStream &iBitStream::operator>>(bool &bit) {
        bit = static_cast<bool>(buf_ & (1u << pos_));
        ++pos_;
        if (pos_ >= CHAR_BIT) {
            buf_ = is_.get();
            pos_ = 0;
        }
        return *this;
    }

    void iBitStream::clear() {
        pos_ = 0;
        buf_ = 0;
    }

    void iBitStream::reopen() {
        close();
        is_.clear();
        is_.seekg(0, std::ios::beg);
        buf_ = is_.get();
    }

    void iBitStream::close() {
        clear();
    }

    iBitStream::operator bool() const {
        return is_.operator bool();
    }

//  Construct end of input stream iterator.
    iBitStream_iterator::iBitStream_iterator() :
            stream_(nullptr), value_(), is_ok_(false) {}

    iBitStream_iterator::iBitStream_iterator(iBitStream &is) :
            stream_(&is), value_(), is_ok_(is) { iter_read(); }

    bool iBitStream_iterator::operator*() const {
        return value_;
    }

    iBitStream_iterator &iBitStream_iterator::operator++() {
        iter_read();
        return *this;
    }

    bool iBitStream_iterator::operator==(const iBitStream_iterator &other) const {
        return (is_ok_ == other.is_ok_) && (!is_ok_ || stream_ == other.stream_);
    }

    bool iBitStream_iterator::operator!=(const iBitStream_iterator &other) const {
        return !operator==(other);
    }

    iBitStream_iterator::operator bool() const {
        return is_ok_;
    }

    void iBitStream_iterator::iter_read() {
        is_ok_ = stream_ && *stream_;
        if (is_ok_) {
            *stream_ >> value_;
        }
    }
}