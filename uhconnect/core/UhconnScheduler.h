#ifndef UHCONNSCHEDULER_H
#define UHCONNSCHEDULER_H

#pragma once

class UhconnScheduler
{
    UhconnScheduler() {};
public:
    UhconnScheduler(int nodeID);
    ~UhconnScheduler();

    int getDestWorkNode(void);
private:
    int max_node;
    int NodeId;
};

#endif