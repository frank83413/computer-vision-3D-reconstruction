# computer-vision-3D-reconstruction

Introduction: Do the 3D reconstruction through the different light sources image.  
  
Read light->Reading picture -> Save 6 images in the same array -> Find the inverse of light -> [L(T)*L].inv*L(T) -> Light inverse*6 image array is normal -> Normalized->Finally, from left add to right (x direction), the first col is set to 0.->The final output.  

![image](https://raw.githubusercontent.com/frank83413/computer-vision-hw-3D-reconstruction/master/result.PNG)  
Left: after Median Filter. Right: before.
