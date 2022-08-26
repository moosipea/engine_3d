# engine_3d

__Issues:__
* Textures warping at certain angles (something related to "perspective correct texturing")
* Vertices of triangles are still rendered when behind frustum, causing artefacts
* Division of the transformed vertex coordinates takes place in the vertex shader, even though, supposedly, OpenGL should take care of that itself
