#default{ finish{ ambient 0.0 diffuse .5 }}              

sky_sphere{ 
    pigment{ 
        gradient <0,1,0>
        color_map{ [0.4 color rgb<.0,0,0>]//White
                   [1.0 color rgb<.8,0,1>]//~Navy
                   
       }
        scale 2 
        rotate<105, 0, 0> 
                      }
} // end of sky_sphere 

#declare T_lines = 
texture {
  pigment {
    gradient x
    color_map {
      [0.00 rgb <0, 1, 1>] 
      [0.06 rgb <0, 1, 1>] 
      [0.06 rgbt 1.0]  
      [1.00 rgbt 1.0]  
    }
  }
}

plane{<0,1,0>, 0   
    texture{pigment{color rgb<0,0,0.5>}}  
    texture { T_lines }
    texture { T_lines rotate 90*y } 
     
}

#declare Pyramid =
 prism {
   conic_sweep  0, 1, 5, 
    <4,4>,<-4,4>,<-4,-4>,<4,-4>,<4,4>    
    pigment { color <0,1,0> }
} 

// Pseudo random mountains

#declare X = -50;    
#declare EndX = 50;  
#declare Step = 5.43;

#while ( X < EndX + Step)
  object{ Pyramid 
    rotate <180, 63, 0>
    translate <X, .5, 0> 
    scale <1, 7.25, 1>       
}

#declare X = X + Step; 
#end 

// ------------------- loop end

#declare Y = -50;   
#declare EndY = 50; 
#declare StepY = 3.71;

#while ( Y < EndY + StepY)
  object{ Pyramid 
    rotate <180, 37, 0>
    translate <Y, .5, 0> 
    scale <1.3, 3.3, -1.3>       
}

#declare Y = Y + StepY; 

#end 

// ------------------- loop end

#declare Z = -50;    
#declare EndZ = 50;  
#declare StepZ = 6.24;

#while ( Z < EndZ + StepZ)
  object{ Pyramid 
    rotate <180, 42, 0>
    translate <Z, .5, 0> 
    scale <1.4, 6.24, -1.25>       
}

#declare Z = Z + StepZ; 
#end 

// ------------------- loop end          

#declare W = -50;    
#declare EndW = 50;  
#declare StepW = 4.35;
                        
#while ( W < EndW + StepW)
  object{ Pyramid 
    rotate <180, 42, 0>
    translate <W, .5, 0> 
    scale <1.35, 4.00, -1.5>       
}

#declare W = W + StepW; 
#end 

// ------------------- loop end