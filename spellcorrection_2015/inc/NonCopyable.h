#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

class NonCopyable
{
protected:
    NonCopyable(){}
    ~NonCopyable(){}

private:
    NonCopyable(const NonCopyable &);
    NonCopyable &operator=(const NonCopyable &);
};

#endif// end _NONCOPYABLE_H
