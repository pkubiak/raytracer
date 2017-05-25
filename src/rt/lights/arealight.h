#pragma once

#include <core/vector.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>

namespace rt {
	class AreaLight : public Light {
		protected:
			Solid* source;
		public:
			AreaLight() {}
			AreaLight(Solid* source);
			virtual LightHit getLightHit(const Point& p) const;
			virtual RGBColor getIntensity(const LightHit& irr) const;
	};
}
