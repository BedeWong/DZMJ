#ifndef MJ_REQUEST_H
#define MJ_REQUEST_H

class MJ_RequestData;

class MJ_Request
{
public:
    MJ_Request();

    virtual send(MJ_RequestData &) = 0;
};

#endif // MJ_REQUEST_H
