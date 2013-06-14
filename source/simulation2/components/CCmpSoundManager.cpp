/* Copyright (C) 2012 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "precompiled.h"

#include "simulation2/system/Component.h"
#include "ICmpSoundManager.h"

#include "simulation2/MessageTypes.h"
#include "simulation2/components/ICmpPosition.h"
#include "simulation2/components/ICmpRangeManager.h"
#include "soundmanager/ISoundManager.h"

class CCmpSoundManager : public ICmpSoundManager
{
public:
	static void ClassInit(CComponentManager& UNUSED(componentManager) )
	{
	}

	DEFAULT_COMPONENT_ALLOCATOR(SoundManager)

	static std::string GetSchema()
	{
		return "<a:component type='system'/><empty/>";
	}

	virtual void Init(const CParamNode& UNUSED(paramNode))
	{
	}

	virtual void Deinit()
	{
	}

	virtual void Serialize(ISerializer& UNUSED(serialize))
	{
		// Do nothing here - sounds are purely local, and don't need to be preserved across saved games etc
		// (If we add music support in here then we might want to save the music state, though)
	}

	virtual void Deserialize(const CParamNode& paramNode, IDeserializer& UNUSED(deserialize))
	{
		Init(paramNode);
	}

	virtual void PlaySoundGroup(std::wstring name, entity_id_t source)
	{
		if ( ! g_SoundManager )
			return;

		CmpPtr<ICmpRangeManager> cmpRangeManager(GetSimContext(), SYSTEM_ENTITY);
		ICmpRangeManager::ELosVisibility vis = cmpRangeManager->GetLosVisibility(source, GetSimContext().GetCurrentDisplayedPlayer());

		if (vis == ICmpRangeManager::VIS_VISIBLE)
		{
			if (source != INVALID_ENTITY)
			{
				CmpPtr<ICmpPosition> cmpPosition(GetSimContext(), source);
				if (cmpPosition && cmpPosition->IsInWorld())
				{
					CVector3D sourcePos = CVector3D(cmpPosition->GetPosition());
					g_SoundManager->PlayAsGroup(name, sourcePos, source);
				}
			}
		}

	}
};

REGISTER_COMPONENT_TYPE(SoundManager)


