#pragma once

// std
#include <memory>

/// Heap-allocated container with unique ownership guaranteed to contain a value
/// It is similar to std::unique_ptr but default-initializes content to T's
/// own default-constructed value, and is guaranteed to contain non-nullptr data.
/// Box supports move semantics.
/// T requires default constructor.
template<typename T>
// we don't require std::is_default_constructible_v<T> which would
// require any user of Box<T> to include T's header, even inside a header
class Box
{
public:
    Box();
    ~Box();

    Box(Box<T>&& other);
    Box<T>& operator=(Box<T>&& other);

    // allow access to non-const internal value, keeping const Box

    T& operator*() const { return *mc_data; }
    T* operator->() const { return mc_data.get(); }

private:

    /* Components (exceptionally private to require usage of * and -> directly) */

    /// Unique pointer to internal data
    std::unique_ptr<T> mc_data;
};

template<typename T>
Box<T>::Box() :
    mc_data(std::make_unique<T>())
{
}

template<typename T>
Box<T>::~Box()
{
}

template<typename T>
Box<T>::Box(Box<T>&& other)
{
    mc_data = std::move(other.mc_data);
}

template<typename T>
Box<T>& Box<T>::operator=(Box<T>&& other)
{
    if (this != &other)
    {
        mc_data = std::move(other.mc_data);
    }

    return *this;
}
