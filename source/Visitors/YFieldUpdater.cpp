#include "YFieldUpdater.h"
#include "../Databases/TerrainDatabase.h"
#include "../Factories/TerrainVisuals.h"
#include "../Nodes/SpatialIndexNode.h"
#include "../Nodes/SpatialIndexCell.h"
#include "../Nodes/TransformGroupNode.h"
//#include "../Nodes/StaticModelNode.h"
#include "../Tools/NodeIterator.h"
#include "../Controllers/TileController.h"
#include "../Controllers/ModelController.h"
#include "../Geometry/Ray3D.h"
#include "../Math/Intersector.h"
#include "../Kernel/Gateway.h"

YFieldUpdater::YFieldUpdater()
{
}

void YFieldUpdater::visit(SpatialIndexCell *cell)
{
  NodeIterator iter(cell->getFirstChild());
  while (!iter.end())
  {
    iter.current()->accept(this);
    iter++;
  }
}

void YFieldUpdater::visit(TransformGroupNode *node)
{
  TileController* tcontroller;
  ModelController* mcontroller;
  Intersector intersector;
  Ray3D ray;
  Tuple4f packet;
  Tuple3f position;
  
  mcontroller = node->getController();
  
  if (mcontroller)
  {
    position = mcontroller->getPosition();
    tcontroller = Gateway::getTerrainDatabase()->getController(Tuple2f(position.x, position.z));
    if (tcontroller)
    {
      ray.setOrigin(Tuple3f(position.x, 1000, position.z));
      ray.setDestination(Tuple3f(position.x, -1000, position.z));
      packet = intersector.intersectTriangles(&ray, tcontroller->getVertices(), Gateway::getTerrainVisuals()->getTileIndices(0), 8);
      mcontroller->translateModel(packet.x, packet.y + 0.05f, packet.z);
    }
  }
}
