#ifndef __CLOUDDETECTION
#define __CLOUDDETECTION

#include "../../terralib5/src/terralib/raster/Raster.h"
#include "../../terralib5/src/terralib/raster/RasterFactory.h"
#include "../../terralib5/src/terralib/raster/Utils.h"
#include "../../terralib5/src/terralib/dataaccess/datasource/DataSource.h"
#include "../../terralib5/src/terralib/dataaccess/datasource/DataSourceFactory.h"
#include "../../terralib5/src/terralib/dataaccess/datasource/DataSourceTransactor.h"
#include <../../terralib5/src/terralib/dataaccess/dataset/DataSetTypeConverter.h>
#include "../../terralib5/src/terralib/memory/DataSet.h"
#include "../../terralib5/src/terralib/memory/DataSetItem.h"
#include "../../terralib5/src/terralib/vp/Utils.h"
#include "../../terralib5/src/terralib/geometry/GeometryProperty.h"

class CloudDetection {

    public:
      CloudDetection();
      ~CloudDetection();

      bool generateLimiarForCloudRaster(std::string& name); //nome da imagem e valor do limiar 

      bool generateLimiarForShadowRaster(std::string& name);

      bool generateSumRasters(te::rst::Raster* rasterCloud, te::rst::Raster* rasterShadow, te::rst::Raster*& rasterOut, std::string& name);

      void openSum(std::string& name);

      te::rst::Raster* generateFilterErosion(std::string& name);

      te::rst::Raster* generateFilterDilation(te::rst::Raster* raster, std::string& name);

      std::vector<te::gm::Geometry*> generateVector(te::rst::Raster* raster);

      te::da::DataSourcePtr createDataSource(std::string& name);

      bool persistDataSet(te::mem::DataSet* dataset, te::gm::Geometry* geom);

      std::auto_ptr<te::da::DataSetType> createOutputDataSetType(int srid, std::string& name);

      void createShape(te::rst::Raster* raster,std::string& name);

      void persistDataSet(te::mem::DataSet* dataset, std::vector<te::gm::Geometry*> geoms);

    };

#endif

