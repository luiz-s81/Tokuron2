#declare RasterLine_Pigment = <0,1,1,0>;  

#macro Raster(RScale, HLine) 
       pigment{ gradient x scale RScale
                color_map{[0.000   color rgbt RasterLine_Pigment]
                          [0+HLine color rgbt RasterLine_Pigment]
                          [0+HLine color rgbt<1,1,1,1>]
                          [1-HLine color rgbt<1,1,1,1>]
                          [1-HLine color rgbt RasterLine_Pigment]
                          [1.000   color rgbt RasterLine_Pigment]
                }    
      }
      
       finish {ambient 0.0 diffuse .9  }
       
#end

#macro Grid_Plane(RasterScale, RasterHalfLine, Background_pigment) 
    plane{<0,1,0>, 0
        texture{Background_pigment}
        texture{Raster(RasterScale,RasterHalfLine)}
        texture{Raster(RasterScale,RasterHalfLine)  rotate<0,90,0> }
     } 
#end