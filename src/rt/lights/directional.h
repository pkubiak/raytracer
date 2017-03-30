#pragma once

#include <rt/lights/light.h>
#include <core/color.h>

namespace rt {
	class DirectionalLight : public Light {
		private:
			Vector direction;
			RGBColor color;

		public:
			DirectionalLight() {}
			DirectionalLight(const Vector& direction, const RGBColor& color);
			virtual LightHit getLightHit(const Point& p) const;
			virtual RGBColor getIntensity(const LightHit& irr) const;
	};

}
