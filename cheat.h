/*coded by https://github.com/soamane*/

#pragma once
#include "classes.hpp"
#include "stdafx.hpp"

namespace add {
	bool IsEnemyTeam(CActor* mActor, CActor* pActor) {
		return ((mActor->GetTeam != pActor->GetTeam || pActor->GetTeam == 0) && pActor != mActor);
	}

	bool ZoomAction(CActor* local_actor, CWeapon* weapon) {
		auto ptr = weapon->GetWeaponGeneral;
		if (ptrs::valid_pointer(ptr)) {
			return  local_actor->GetCurrentClass == 2 || local_actor->GetCurrentClass == 3 ? ptr->GetWeaponState() == 2 : true;
		}
	}

	int CalculateDistance(Vec3_tpl<float> m_Position, Vec3_tpl<float> e_Position) {
		return nocrt::_sqrt(((m_Position.x - e_Position.x) * (m_Position.x - e_Position.x)) +
			((m_Position.y - e_Position.y) * (m_Position.y - e_Position.y)) +
			((m_Position.z - e_Position.z) * (m_Position.z - e_Position.z)));
	}

	void itoa_strcat(CActor* e_actor, char* health, char* armor, char* distance, int& render_dist, char name_health[32], char name_armor[32], char name_distance[32]) {
		nocrt::__itoa(e_actor->GetHealth(), health, 10);
		nocrt::__itoa(e_actor->GetHealth(), health, 10);
		nocrt::__itoa(e_actor->GetArmor(), armor, 10);
		nocrt::__itoa(render_dist, distance, 10);

		nocrt::str_cat(health, name_health);
		nocrt::str_cat(armor, name_armor);
		nocrt::str_cat(distance, name_distance);
	}

	void watermark(CRenderer* p_renderer) {

		p_renderer->DrawTextA(10, 10, 1.2f, green, 0, "AELA v1.0.0b");

	}
}
namespace functions {

	void Base() {
		CSystemGlobalEnvironment* p_env = CSystemGlobalEnvironment::Instance();

		CGame* p_game = p_env->GetGame;
		if (!ptrs::valid_pointer(p_game)) {
			return;
		}

		CSVars* p_cvars = p_game->GetCSvars;
		if (!ptrs::valid_pointer(p_cvars)) {
			return;
		}

		if (p_cvars->IsInRoom(3) == true || p_cvars->IsInRoom(3) == false) {

			CRenderer* p_renderer = p_env->GetRenderer;
			if (!ptrs::valid_pointer(p_renderer)) {
				return;
			}

			CEntitySystem* p_entity_system = p_env->GetEntitySystem;
			if (!ptrs::valid_pointer(p_entity_system)) {
				return;
			}

			CGameFramework* p_framework = p_game->GetFramework();
			if (!ptrs::valid_pointer(p_framework)) {
				return;
			}

			CActorSystem* p_actor_system = p_framework->GetActorSystem();
			if (!ptrs::valid_pointer(p_actor_system)) {
				return;
			}

			add::watermark(p_renderer);

			CActor* p_actor;
			if (p_framework->GetClientActor(&p_actor)) {
				CItem* p_item = p_actor->GetCurrentItem();
				if (!ptrs::valid_pointer(p_item)) {
					return;
				}

				CWeapon* p_weapon = p_item->GetWeapon();
				if (!ptrs::valid_pointer(p_weapon)) {
					return;
				}

				CWeaponParameters* p_weapon_params = p_weapon->GetWeaponGeneral->GetWeaponParameters();
				if (!ptrs::valid_pointer(p_weapon_params)) {
					return;
				}
				/*stats example*/

				for (int enum_iterator = eGS_RclAtt; enum_iterator < eGS_RclSmth; enum_iterator++) {
					RELEASE_WEAPON_PARAMS(p_weapon_params, enum_iterator, 0.f);
				}
				/*stats example*/

				CEntityIterator* iterator = p_entity_system->GetEntityIterator();
				if (!ptrs::valid_pointer(iterator)) {
					return;
				}

				while (CEntity* p_entity = iterator->Iterator()) {
					if (CActor* e_actor = p_framework->GetActorSystem()->GetActor(p_entity->GetEntityId())) {

						if (GetKeyState(VK_F1))p_weapon->GetWeaponGeneral->UseDefibrillator(p_actor->GetEntityId, false);

						if (add::IsEnemyTeam(p_actor, e_actor) && !e_actor->IsDead()) {

							UINT get_class_p_actor = p_actor->GetCurrentClass;
							UINT BoneId = get_class_p_actor == 2 || get_class_p_actor == 3 ? 1 : 36;
							auto entity_item = e_actor->GetCurrentItem();
							if (!ptrs::valid_pointer(entity_item)) {
								return;
							}
							auto entity_weapon = entity_item->GetWeapon()->GetWeaponGeneral;
							if (!ptrs::valid_pointer(entity_weapon)) {
								return;
							}

							Vec3_tpl<float> pos = p_entity->GetPos();
							auto render_dist = add::CalculateDistance(pos, p_renderer->GetViewCamera);

							UINT class_id = e_actor->GetCurrentClass;

							CSkeletonPose* p_skeleton = p_entity->GetCharacterInstance(0)->GetSketetonPose();
							if (!ptrs::valid_pointer(p_skeleton)) {
								return;
							}

							add::itoa_strcat(e_actor, health, armor, distance_, render_dist, name_health, name_armor, name_distance);

							switch (class_id) {
							case 0:
							{
								class_name = "rifle";
							} break;
							case 1:
							{
								class_name = "sed";
							} break;
							case 2:
							{
								class_name = "sniper";
							} break;
							case 3:
							{
								class_name = "medic";
							} break;
							case 4:
							{
								class_name = "engineer";
							} break;
							}
							/*esp*/
							if (p_renderer->WorldToScreen(pos, screen)) {
								FLOAT pos_x = screen.x, pos_y = screen.y;

								p_renderer->DrawTextA(pos_x, pos_y, 1.2f, green, 1, health);
								p_renderer->DrawTextA(pos_x, pos_y + 10, 1.2f, blue, 1, armor);
								p_renderer->DrawTextA(pos_x, pos_y + 20, 1.2f, red, 1, distance_);
								p_renderer->DrawTextA(pos_x, pos_y + 30, 1.2f, white, 1, class_name);

								p_renderer->DrawTextA(pos_x, pos_y + 40, 1.2f, white, 1, entity_weapon->GetWeaponState() == 2 ? "zoomed" : "");
							}
							/*esp*/

							/*skeleton*/
							for (int i = 0; i < ARRAYSIZE(bones); i++) {
								if (p_renderer->WorldToScreen(p_entity->GetBonePos(p_skeleton, bones[i]), screen)) {
									FLOAT pos_x = screen.x, pos_y = screen.y;

									p_renderer->DrawTextA(pos_x, pos_y, 1.8f, red, 1, ".");
									if (bones[i] == 36 || bones[i] == 35) {
										p_renderer->DrawTextA(pos_x, pos_y, 1.8f, yellow, 1, ".");
									}
								}
							}
							/*skeleton*/
							if (CActor* trigger_actor = p_actor_system->GetActor(p_actor->GetGameObject->GetWorldQuery()->GetLookAtEntityId())) {
								if (add::IsEnemyTeam(p_actor, trigger_actor) && !trigger_actor->IsDead() && add::ZoomAction(p_actor, p_weapon)) {

								}
							}

						}
					}
				}
			}
		}
	}
}