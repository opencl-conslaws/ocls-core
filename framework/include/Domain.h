#ifndef DOMAIN_H
#define DOMAIN_H

#include <cstddef>
#include <vector>

namespace ocls {

/** \ingroup main_api
*  @{
*/
class Domain{
public:
    enum Dim{
        X = 1, Y = 2, Z = 3
    };

    struct Boundary{
        Boundary(){
            min = 0.0f;
            max = 0.0f;
        }
        float min;
        float max;
    };
    
    /**
     * Create a new domain of 1, 2 or 3 dimensions
     */
    Domain(const unsigned short& dimensions_ = 1);

    /**
    * Copy const constructor
    */
    //Domain(const Domain& other);

    /**
    * Copy constructor
    */
    //Domain(Domain& other);

    /**
     * Destructor
     */
    ~Domain();

    /**
     * Set the bounds for the domain
     */
    void setBounds(const float& left_, const float& right_);
    void setBounds(const float& left_, const float& right_, const float& bottom_, const float& top_);
    void setBounds(const float& left_, const float& right_, const float& bottom_, const float& top_, const float& front_, const float& back_);
    
    /**
     * Set the mesh refinement in cell count
     */
    void setMesh(const size_t& x_);
    void setMesh(const size_t& x_, const size_t& y_);
    void setMesh(const size_t& x_, const size_t& y_, const size_t& z_);
    
    /**
     * Set the number of ghost cells
     */
    void setGhostCells(const unsigned short& ghost_cells_);
    
    /**
     * Get the domain bounds
     */
    Boundary* getBounds();
    
    /**
     * Get the domain bounds
     */
    Boundary getBounds(Dim dimension) const;
    
    /**
     * Get the mesh refinement
     */
    size_t* getMesh();
    
    /**
     * Return the mesh refinement in a dimensional direction.
     */
    size_t getMesh(Dim dimension) const;
    
    /**
     * Return the mesh refinement in a dimensional direction.
     */
    size_t getMeshWithGC(Dim dimension) const;

    /**
    * Return the delta value
    */
    float getDelta(Dim dimension) const;
    
    /**
     * Return the number of dimensions
     */
    unsigned short getDimensions() const;
    
    /**
     * Return the number of dimensions
     */
    unsigned short getGhostCells() const;
    
private:
    unsigned short m_dimensions;

    Boundary m_bounds[3];
    size_t m_mesh[3];
    //std::vector<Boundary> m_bounds;
    //std::vector<size_t> m_mesh;
    
    unsigned short m_ghost_cells;
};
/** @} */

} //cls

#endif //PIPELINE_H
