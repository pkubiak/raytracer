#pragma once

#include <core/point.h>
#include <core/color.h>
#include <rt/lights/light.h>
#include <rt/textures/texture.h>
#include <core/vector.h>
#include <cmath>

namespace rt {

	class ProjectiveLight : public Light {
		protected:
			Point position;
			Vector direction, up, left;
      Texture* texture;
      float angle, factor;

		public:
			ProjectiveLight(const Point& position_, const Vector& direction_, const Vector& up_, Texture* texture_, float angle_ = M_PI/2, float factor = 100.0);

			virtual LightHit getLightHit(const Point& p) const;
			virtual RGBColor getIntensity(const LightHit& irr) const;
	};
}
