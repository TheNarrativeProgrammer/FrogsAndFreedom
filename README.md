# Unreal Project Base 5.4.3

### ------------------  ESSENTIAL SETUP  ----------------------
_*All the "Essential Setup" section has been setup for you already!_ 

**Project Settings**  
Search "Virtual Textures"  
Enable Virtual Textures Support

**Plugins**  
Search "Water"  
Enable Virtual Textures Support

**DefaultEngine.ini**  
[/Script/Engine.RendererSettings]  
r.Nanite.AllowTessellation=1  
r.Nanite.Tessellation=1  

**Results Node**  
Enable Tessellation  
Use with Nanite 
 
### --------------  MATERIAL INSTANCE SETTINGS  ----------------
**_Must be adjusted for every MI!_**     
Material Property Overrides > Displacement Scaling >  
Magnitude > 2 or as needed ( is the max displacement )  
Center = 0

### --------------  TEXTURE PACKING FORMAT  ----------------

BaseColor [RGB] Opacity [A] ( BC7 )  
Metal [R] Height [G] Emissive [B] Roughness [A] ( BC7 )  
NormalDX [RGB] ( BC5 )  

### ---------------  SHADER FEATURE LIST  ---------------------

**[ 01 ]** Blend Layer Expansion ( x4 )  
**[ 02 ]** Vertex Painting or Procudural Dusting Blend Styles  
**[ 03 ]** Authored UVs or Projected UVs  
**[ 04 ]** Height Blending  
**[ 05 ]** Nanite Tesselation Displacement  
**[ 06 ]** PBR ( tweaking ) Color Recolor | Normal Intensity | etc...  


### ---------------------------------------------------------------

# Frogs and Freedom
Frogs and Freedom is a first-person movement-based boomer shooter where you play as Grant Boom, a rugged soldier tasked with infiltrating a top-secret underground bunker where an experiment has gone wrong, turning all the scientists inside to rabid mutant frogs. Your goal is to rocket jump and blast your way to get to the frog spawner and exterminate it, then escape before the facility caves in.

### Documentation  
(https://drive.google.com/drive/folders/16IGBfi463B6u0Di4oUKc-jJiJBrIuOaY?usp=drive_link)

### Gameplay Controls  
**Action**           | **Keyboard Control**  
Move                 | WASD  
Aim                  | Mouse Look  
Shoot                | Mouse L  
Air Burst            | Mouse R  
Interact             | E  

### Debug Controls  
**Action**           | **Keyboard Control**  
TBD

### Team  
Marcus Chala - Project Manager  
William Cashin - Artist  
Edric Anthony Trisno - Level Designer  
Reid Gillis - Programmer  
Jordan Benson - Programmer  
Connor McNeilly - Programmer  
