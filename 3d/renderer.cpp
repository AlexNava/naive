#include "renderer.hpp"
#include "glm/glm.hpp"
#include "screen.hpp"
#include "scene.hpp"

Renderer::Renderer(Screen *targetScreen)
{
    m_pScreen = targetScreen;
}

void Renderer::setPScene(Scene *newPScene)
{
    m_pScene = newPScene;
}

void Renderer::render()
{
    // cull objects in the scene with grid
    //   > bonus: with bsp or quad/octree
    // then:

    // 0 - naivest
    // get triangles list from unculled objects
    // merge lists in a single one
    // z sort and draw triangles
    // optimizations:
    //   > hints to not sort (e.g. background)
    //     > better: set "layers" (eg draw background first, then outside world, then cockpit and only sort relative to layer)
    //   > do partial sorts (e.g. stop when z is close enough)
    //   > keep list between frames, adding and removing triangles as the camera moves
    //     > list will always be partially sorted, allowing to use bubble sort and incomplete sorts (only few passes per frame)

    // 1
    // query distance of objects in the scene
    // sort objects
    // draw all of them
    //   > get locally sorted list of triangles
    //     > precalc sorted lists for N directions
    //   > draw taking material flags into account

    // 1.5
    // sort objects
    // project bounding box and cull objects if they "fully" overlap others in foreground
    // do the rest of the sorting and drawing

    // 2
    // iterate on unsorted objects
    //   > get locally sorted list of triangles
    //   > put triangles in bins for bucket sort. distance will be that of object + offsets?
    //   > draw buckets in order, do not bother sorting them :)

    // 3
    // iterate on unsorted objects
    // draw them with cache advantage for texture etc
    // implement rudimentary z buffer
    // only exclude and sort transparent triangles

    // x
    // sort triangles considering projection overlap
    //   > if no overlap then never swap
    //   > can fail.

    // y
    // split sorting of triangles in screen regions
    //   > can cause artifacts in region borders

    // grezzotest: only render unsorted objects

}

