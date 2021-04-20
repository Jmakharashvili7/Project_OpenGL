An openGL application that loads most 3D models and renders them in 3D. 

There are also Index buffer, Vertex buffer, and vertex array classes. All openGL calls are either wrappers in GLCall macro or have their own error checking setup to make sure errors are easier to track down. The macro prints out the line and file where the error happened and the error code to check for the error. 

The application uses Modern OpenGL which means it requires proper shaders and vertex and index setup. Currently the camera moves with both mouse and keyboad input. Lighting has its own class which has two diffrent types of lighting available. 

The default loaded model is a 3d base model of a person. There is also a backpack model available that can be directly loaded in. The backpack is setup so it doesnt need a lighting class to be lit up. The reason for this is the fact that it was used before the lighting class and was testing importing 3D Models.
