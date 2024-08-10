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
    virtual void playMedia() = 0;
    virtual void stopMedia() = 0;
    virtual void pauseMedia() = 0;

    // Non-Pure Virtual Functions

};

#endif // MEDIA_H
