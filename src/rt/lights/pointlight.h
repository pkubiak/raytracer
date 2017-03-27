#pragma once

#include <core/point.h>
#include <core/color.h>
#include <rt/lights/light.h>

namespace rt {

	class PointLight : public Light {
		private:
			Point position;
			RGBColor intensity;
			
		public:
			PointLight();
			PointLight(const Point& _position, const RGBColor& _intensity);
			virtual LightHit getLightHit(const Point& p) const;
			virtual RGBColor getIntensity(const LightHit& irr) const;
	};
}
