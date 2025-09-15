#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <memory>
#include <stdexcept>


template <typename T>


class Vector {


private:

    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

    constexpr void reallocate(size_t new_capacity) {

        T* new_data = nullptr;

        try {

            new_data = new T[new_capacity];
        
            for (size_t i = 0; i < size_; ++i) {

                new_data[i] = std::move_if_noexcept(data_[i]);
            }
        
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }   
        
            //exception handling
            catch (...) { 

            delete[] new_data; 
            throw; 
            }
    }

public:

    // Constructors
    constexpr Vector() noexcept = default;
    
    constexpr explicit Vector(size_t size) : size_(size), capacity_(size) {

        data_ = new T[size];
    }
    
    constexpr Vector(size_t size, const T& value) : size_(size), capacity_(size) {

        data_ = new T[size];

        for (size_t i = 0; i < size; ++i) {

            data_[i] = value;
        }
    }
    
    constexpr Vector(std::initializer_list<T> init) : 

        size_(init.size()), capacity_(init.size()) {

        data_ = new T[capacity_];
        std::copy(init.begin(), init.end(), data_);
    }

    // copy constructor
    constexpr Vector(const Vector& other) : 

        size_(other.size_), capacity_(other.capacity_) {

        data_ = new T[capacity_];

        for (size_t i = 0; i < size_; ++i) {

            data_[i] = other.data_[i];
        }
    }

    //move constructor
    constexpr Vector(Vector&& other) noexcept : 

        data_(other.data_), size_(other.size_), capacity_(other.capacity_) {

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    //copy assignment
    constexpr Vector& operator=(const Vector& other) {

        if (this != &other) {

            delete[] data_;
            
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            
            for (size_t i = 0; i < size_; ++i) {

                data_[i] = other.data_[i];
            }
        }

        return *this;
    }

    //move assignment
    constexpr Vector& operator=(Vector&& other) noexcept {

        if (this != &other) {

            delete[] data_;
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        return *this;
    }

    //destructor
    ~Vector() {

        delete[] data_;
    }


    // Element access
    constexpr T& operator[](size_t index) {

        return data_[index];
    }
    
    constexpr const T& operator[](size_t index) const {

        return data_[index];
    }
    
    constexpr T& at(size_t index) {

        if (index >= size_) {

            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }
    
    constexpr const T& at(size_t index) const {

        if (index >= size_) {

            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }
    
    constexpr T& front() { return data_[0]; }
    constexpr const T& front() const { return data_[0]; }
    constexpr T& back() { return data_[size_ - 1]; }
    constexpr const T& back() const { return data_[size_ - 1]; }
    constexpr T* data() noexcept { return data_; }
    constexpr const T* data() const noexcept { return data_; }


    // Capacity
    constexpr bool empty() const noexcept { return size_ == 0; }
    constexpr size_t size() const noexcept { return size_; }
    constexpr size_t capacity() const noexcept { return capacity_; }


    // Modifiers
    constexpr void push_back(const T& value) {

        if (size_ >= capacity_) {

            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        }

        data_[size_++] = value;
    }
    
    constexpr void push_back(T&& value) {

        if (size_ >= capacity_) {

            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        }

        data_[size_++] = std::move(value);
    }
    
    template <typename... Args>

    constexpr void emplace_back(Args&&... args) {

        if (size_ >= capacity_) {

            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        }

        data_[size_++] = T(std::forward<Args>(args)...);
    }
    
    constexpr void pop_back() {

        if (size_ > 0) {

            --size_;
        }
    }
    
    constexpr void clear() noexcept {

        size_ = 0;
    }
    
    constexpr void reserve(size_t new_capacity) {

        if (new_capacity > capacity_) {

            reallocate(new_capacity);
        }
    }
    
    constexpr void resize(size_t new_size, const T& value = T()) {

        if (new_size > capacity_) {

            reallocate(new_size);
        }
        
        if (new_size > size_) {

            for (size_t i = size_; i < new_size; ++i) {

                data_[i] = value;
            }
        }

        size_ = new_size;
    }


    // Iterators
    constexpr auto begin() noexcept -> T* { return data_; }
    constexpr auto begin() const noexcept -> const T* { return data_; }
    constexpr auto end() noexcept -> T* { return data_ + size_; }
    constexpr auto end() const noexcept -> const T* { return data_ + size_; }


    // Comparison
    constexpr bool operator==(const Vector& other) const {

        if (size_ != other.size_) return false;

        for (size_t i = 0; i < size_; ++i) {

            if (data_[i] != other.data_[i]) return false;
        }

        return true;
    }
    
    constexpr bool operator!=(const Vector& other) const {

        return !(*this == other);
    }
};


// Template function with auto return type
template <typename T>

auto sum(const Vector<T>& vec) -> decltype(std::declval<T>() + std::declval<T>()) {

    auto result = T{};

    for (const auto& element : vec) {

        result += element;
    }

    return result;
}


// Template transformation function
template <typename T, typename Func>

auto transform(const Vector<T>& vec, Func func) -> Vector<decltype(func(std::declval<T>()))> {

    using ResultType = decltype(func(std::declval<T>()));
    Vector<ResultType> result;
    result.reserve(vec.size());
    
    for (const auto& element : vec) {

        result.push_back(func(element));
    }
    
    return result;
}

int main() {

    // Test basic functionality
    Vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "Vector elements: ";

    for (const auto& elem : v1) {

        std::cout << elem << " ";
    }

    std::cout << "\n";
    
    // Test push_back
    v1.push_back(6);
    std::cout << "After push_back(6): ";

    for (const auto& elem : v1) {

        std::cout << elem << " ";
    }

    std::cout << "\n";
    
    // Test template function with auto
    auto total = sum(v1);
    std::cout << "Sum of elements: " << total << "\n";
    
    // Test transform function
    auto doubled = transform(v1, [](int x) { return x * 2; });
    std::cout << "Doubled elements: ";

    for (const auto& elem : doubled) {

        std::cout << elem << " ";
    }

    std::cout << "\n";
    
    
    Vector<int> v2 = v1; // Test Copy constructor
    Vector<int> v3 = std::move(v1); // Test Move constructor
    std::cout << "Copied vector: ";

    for (const auto& elem : v2) {

        std::cout << elem << " ";
    }

    std::cout << "\n";
    
    std::cout << "Moved vector: ";

    for (const auto& elem : v3) {

        std::cout << elem << " ";
    }

    std::cout << "\n";
    
    // Test constexpr capabilities at compile time
    // We can't use constexpr with dynamic allocation at runtime,
    // but we can demonstrate constexpr member functions
    const Vector<int> const_vec = {10, 20, 30};
    
    // These will work because they're constexpr functions called at runtime
    std::cout << "Const vector size: " << const_vec.size() << "\n";
    std::cout << "First element: " << const_vec[0] << "\n";
    
    
    return 0;
}

