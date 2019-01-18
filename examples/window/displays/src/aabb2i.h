#pragma once

//------------------------------------------------------------------------
struct aabb2i 
{
   public:
      aabb2i() 
         : left(0)
         , right(0)
         , top(0)
         , bottom(0)
      {}

      inline int get_width() const { return right - left; }
      inline int get_height() const { return bottom - top; }

   public:
      int left;
      int right;
      int top;
      int bottom;
};