#ifndef UHCONNVOXORFACTORY_H
#define UHCONNVOXORFACTORY_H

#pragma once

class UhconnWorkNode;
class UhconnVoxor;
class UhconnDco;
class DcoFactory;
class UhconnVoxorFactory
{
    UhconnVoxorFactory();
    ~UhconnVoxorFactory();
    UhconnVoxorFactory(const UhconnVoxorFactory&);
    UhconnVoxorFactory& operator = (const UhconnVoxorFactory&); 
public:
    //@brief 单例模式，c++11下线程安全
    static UhconnVoxorFactory& getInstance(void) {
        static UhconnVoxorFactory instance;
        return instance;
    };

    /**
     * @brief 服务启动必须首先执行 setupLocalWorkEnvironment
     *          执行结果会创建一个 worknode，并分配地址
     * 
     */
    void setupLocalWorkEnvironment(DcoFactory* df, int sn = 0);
    UhconnVoxor* createVoxor(UhconnDco* dco);
    UhconnWorkNode* getLocalWorkNode(void);
    DcoFactory* getLocalFactory(void);
    void setLocalFactory(DcoFactory* nf);
    DcoFactory* switchLocalFactory(DcoFactory* nf);
private:
    UhconnWorkNode* Local_Worknode;
    DcoFactory* local_factory;
};

// UhconnWorkNode* getLocalWorkNode(void);
// DcoFactory* getLocalFactory(void);


#endif