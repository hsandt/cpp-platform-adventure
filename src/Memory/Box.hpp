#pragma once

// std
#include <concepts>
#include <memory>

/// Heap-allocated container with unique ownership guaranteed to contain a value (via a non-nullptr
/// unique_ptr<T>)
/// It is similar to std::unique_ptr but enforces construction of the contained object of type T.
/// If T has a default constructor, the box also defines a default constructor relying on it.
/// Otherwise, you need to explicitly construct your Box by passing the same arguments as you
/// would to construct an object of type T (arguments are forwarded to std::make_unique<T>).
/// Box can be constructed from a moved unique_ptr, but it cannot be moved to another Box
/// (as it would break the guarantee that the moved Box is still valid after move, as it would
/// lose its ownership).
template<typename T>
class Box
{
public:
    /// Construct Box<T> with arguments forwarded to T constructor
    // we don't `requires std::constructible_from<T, Args...>` as it would
    // require any user of Box<T> to include T's header, even inside a header
    template<typename... Args>
    Box(Args&&... args);

    /// Construct Box<T> from moved unique_ptr<U>. Transfers ownership of U instance to the box.
    /// U can be T itself, or a convertible type. std::unique_ptr has its own private concept
    /// __safe_conversion_up which is more advanced and checks array types, but we use our own
    /// `requires` to guide user and have a slightly better error message (unfortunately it doesn't
    /// mention convertible_to, but is much shorter than the error message without)
    template<typename U>
        requires std::convertible_to<U*, T*>
    explicit Box(std::unique_ptr<U> data);

    ~Box();

    // Allow access to non-const internal value, keeping const Box

    T& operator*() const { return *mc_data; }
    T* operator->() const { return mc_data.get(); }
    T* get() const { return mc_data.get(); }

private:

    /* Components */

    /// Unique pointer to internal data
    const std::unique_ptr<T> mc_data;
};

template<typename T>
template<typename... Args>
Box<T>::Box(Args&&... args) :
    mc_data(std::make_unique<T>(std::forward<Args>(args)...))
{
}

template<typename T>
template<typename U>
    requires std::convertible_to<U*, T*>
Box<T>::Box(std::unique_ptr<U> data) :
    mc_data(std::move(data))
{
}

template<typename T>
Box<T>::~Box()
{
}
