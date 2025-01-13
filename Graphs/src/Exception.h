//
//  Exception.hpp
//  Graphs
//
//  Created by Tigran on 06.01.24.
//

#ifndef Exception_hpp
#define Exception_hpp

#include "string"

class Exception {
private:
    const std::string message_;
public:
    Exception(const std::string& message):message_(message) {};
    Exception(std::string&& message):message_(std::move(message)) {};
    const char* message() const;
};

inline const char* Exception::message() const {
    return message_.c_str();
}

class MemoryException : public Exception {
public:
    using Exception::Exception;
};

class IdentifierException : public Exception {
public:
    using Exception::Exception;
};

class ElementException : public Exception {
public:
    using Exception::Exception;
};


class ConflictException : public Exception {
public:
    using Exception::Exception;
};

class FileException : public Exception {
public:
    using Exception::Exception;
};

#endif /* Exception_hpp */
