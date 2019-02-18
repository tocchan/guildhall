<shader id="default/unlit_transparent">

   <!-- A "pass" is our current Shader.  It is all the state of rendering pipeline.
        Later on, we will add support for multiple pass shaders, so having this syntax to be forward compatible -->
   <pass src="shader/src/hlsl/unlit.hlsl">
      <vert entry="VertexFunction" />
      <frag entry="FragmentFunction" />

      <!-- depth will use "always" and write="false" by default to maintain 2D functionality -->
      <!-- That is effectively saying depth is disabled -->
      <!-- for 3D, you want to set "write" to true and test to "lequal" -->
      <!-- test:  test: never, less, equal, lequal, greater, gequal, not, always -->
      <depth
         write="true"
         test="lequal" />


      <!-- I'm open to options here.  You can expose "op" and "factors", or support a "mode", or both.  But at least
           have some way to control blending for color -->
      <blend>
         <!-- setting this to "opaque".  We usually don't render alpha if we're writing depth -->
         <color op="add" src="one" dst="zero" /> 
         <!-- color mode="opaque" / -->
      </blend>
   </pass>
</shader>
