#ifndef COMPOINTER_H
#define COMPOINTER_H

#include <windows.h>

template<class T> class ComPointer
{
    public:
        explicit ComPointer(T * t = 0) : m_t(t)
        {
        }

        explicit ComPointer(const IID & clsid, const IID & iid) : m_t(0)
        {
            ::CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER, iid,
                               reinterpret_cast<void **>(&m_t));
        }

        explicit ComPointer(IUnknown * _unk, const GUID & guid) : m_t(0)
        {
            if (_unk) {
                _unk->QueryInterface(guid, reinterpret_cast<void **>(&m_t));
            }
        }

        ComPointer(const ComPointer<T> & other) : m_t(other.m_t)
        {
            if (m_t) {
                m_t->AddRef();
            }
        }

        ComPointer<T> & operator=(const ComPointer<T> & other)
        {
            if (other.m_t) {
                other.m_t->AddRef();
            }
            if (m_t) {
                m_t->Release();
            }
            m_t = other.m_t;
            return *this;
        }

        T * operator->() const
        {
            return m_t;
        }

        operator T * () const
        {
            return m_t;
        }

        T ** pparam()
        {
            if (m_t) {
                m_t->Release();
                m_t = 0;
            }
            return &m_t;
        }

        bool operator==(const ComPointer<T> & other) const
        {
            return m_t == other.m_t;
        }

        bool operator!=(const ComPointer<T> & other) const
        {
            return m_t != other.m_t;
        }

        ~ComPointer()
        {
            if (m_t) {
                m_t->Release();
            }
        }

    private:
        T * m_t;
};

#endif // COMPOINTER_H
