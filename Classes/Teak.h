#ifndef __TEAK_H__
#define __TEAK_H__

class Teak {
public:
    static Teak* getInstance();

    void identifyUser(const char* userId);
};

#endif // __TEAK_H__
