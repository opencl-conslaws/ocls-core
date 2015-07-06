#ifndef MODULE_H
#define MODULE_H

namespace ocls {

class Framework;
    
/**
 *
 *
 */
class Module{
public:
    /**
     * Constructor
     */
    Module() {};
    /**
     * Virtual destructor for abstract class
     */
    virtual ~Module() {};
    
    /**
     * Called when the module is initialized by the framework
     */
    virtual void onInitialize(Framework* framework) = 0;
    /**
     * Called when the module is destoryed by the framework
     */
    virtual void onQuit(Framework* framework) = 0;
    
    /** 
     * Called at the beginning of every simulation timestep
     */
    //virtual void onTimestepBegin(Framework* framework) = 0;
    
    /**
     * Called at the end of every simulation timestep
     */
    //virtual void onTimestepEnd(Framework* framework) = 0;
};

} //cls

#endif //MODULE_H