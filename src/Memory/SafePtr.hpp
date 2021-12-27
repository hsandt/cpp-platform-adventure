#pragma once

/// Safe Pointer class: wrapper around raw pointer, used for:
/// - raw pointers to non-owned objects provided by third-party API
/// - objects that may or may not exist, as a substitute for optional reference wrapper
/// It provides non-const access to the referenced value even when const itself,
/// allowing user to store it as a public const member, for easy but safe access
/// (the pointer itself cannot be changed, but the referenced value can be).
/// #if PPK_ASSERT_ENABLED: throws on invalid dereference
/// Consider disabling PPK_ASSERT_ENABLED in Release if this impacts performance too much
/// (just wrap the asserts with #if DEBUG).
/// It should satisfy the named requirement of NullablePointer
template<typename T>
class SafePtr
{
public:
    /// Construct default SafePtr<T> as containing null pointer
    SafePtr() : mc_data(nullptr) {}

    /// Construct SafePtr<T> from raw pointer
    /// Allow implicit conversion as this is used a lot
    SafePtr(T* data) : mc_data(data) {}

    /// Assign raw pointer to SafePtr<T> directly
    SafePtr& operator=(T* data);

    /// Return true iff contains data
    /// Check this before deferencing
    explicit operator bool() const { return mc_data != nullptr; }

    // Allow access to non-const internal value, keeping const SafePtr

    T& operator*() const;
    T* operator->() const;
    T* get() const { return mc_data; }

private:

    /* Components */

    /// Raw pointer to internal data
    T* mc_data;
};

template<typename T>
SafePtr<T>& SafePtr<T>::operator=(T* data)
{
    mc_data = data;
    return *this;
}

template<typename T>
T& SafePtr<T>::operator*() const
{
    PPK_ASSERT_ERROR(mc_data, "SafePtr dereference *: no data");
    return *mc_data;
}

template<typename T>
T* SafePtr<T>::operator->() const
{
    PPK_ASSERT_ERROR(mc_data, "SafePtr dereference ->: no data");
    return mc_data;
}

