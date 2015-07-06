#ifndef PLOT_H
#define PLOT_H

#include "Framework.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class Plot {
public:
    Plot(ocls::Module* module, ocls::RenderContext* context, std::string name, ocls::Data* data, ocls::Domain* domain);
    virtual ~Plot(){};

    virtual void update(ocls::Data* data = NULL) = 0;

    size_t getID();

    static void startSync();
    static void endSync();

protected:
    friend class SimplePlot;
    void requestRepaintWindow();

protected:
    size_t m_id;
    static size_t id_seed;

    ocls::RenderContext* m_context;
    ocls::RenderWindow* m_window;

    volatile bool m_closed;

    ocls::Data* m_data;
    ocls::Domain* m_domain;

    ocls::Module* m_module;

    static bool syncing;
    static glm::mat4 transform;
};

#endif