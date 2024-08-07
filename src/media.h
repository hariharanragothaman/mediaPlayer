#ifndef MEDIA_H
#define MEDIA_H

#include <string>

class Media
{
protected:
    std::string file_path;
public:
    // Constructors
    explicit Media(const std:: string& file_path);
    Media();

    // Default destructor
    virtual ~Media() = default;

    // Pure Virtual Functions
    virtual void displayInfo() const = 0; // This function needs to be implemented by derived class to make it instantiable.

    // Non-Pure Virtual Functions
    virtual void playMedia() = 0;

};

#endif // MEDIA_H
