#pragma once

struct Dog {
    virtual ~Dog()                = default;
    virtual const char* GetName() = 0;
    virtual void        Bark()    = 0;
};
