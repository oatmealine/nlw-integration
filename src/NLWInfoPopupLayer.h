#ifndef NLWINFOLAYER_H
#define NLWINFOLAYER_H

#include "Geode/binding/GJGameLevel.hpp"
#include "Geode/ui/Popup.hpp"
#include "NLWRating.h"

class NLWInfoPopupLayer : public geode::Popup<GJGameLevel*, NLWRating*> {
	protected:
		GJGameLevel* m_level;
		NLWRating* m_rating;

		bool setup(GJGameLevel* level, NLWRating* rating);
		void onOpen(CCObject* sender);
		void openTierLevels(CCObject* sender);

	public:
		static NLWInfoPopupLayer* create(GJGameLevel* level, NLWRating* rating);
};

#endif // NLWINFOLAYER_H