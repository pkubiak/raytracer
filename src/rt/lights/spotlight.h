#pragma once

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/lights/pointlight.h>

namespace rt {
	class SpotLight : public PointLight {
		private:
			float angle, exp;
			Vector direction;
			
		public:
			SpotLight() {}
			SpotLight(const Point& position, const Vector& direction, float angle, float exp, const RGBColor& intensity);
			virtual RGBColor getIntensity(const LightHit& irr) const;
	};

}
