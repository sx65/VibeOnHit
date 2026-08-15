#include "player.h"

#include <unordered_set>

#include "../game/game.h"

namespace rbx {
    rbx::instance player::get_team( void ) const noexcept {
        return rbx::instance();
    }

    rbx::model_instance player::get_model_instance( void ) const noexcept {
        const auto address = get_address();

        if ( !address )
            return rbx::model_instance();

        return memory.read<rbx::model_instance>( address + Offsets::Player::model_instance );
    }

    std::vector<std::string> player_body_parts::get_part_names() const noexcept {
        std::vector<std::string> part_names;
        
        switch ( game->place_info.place_id ) {
        default:
            part_names = { 
                "Head",
                "HumanoidRootPart",

                "UpperTorso",
                "LowerTorso",
                
                "RightUpperArm",
                "RightLowerArm",
                "RightHand",
                "LeftUpperArm",
                "LeftLowerArm",
                "LeftHand",
                
                "RightUpperLeg",
                "RightLowerLeg",
                "RightFoot",
                "LeftUpperLeg",
                "LeftLowerLeg",
                "LeftFoot",

                "Torso",
                
                "Right Arm",
                "Left Arm",

                "Right Leg",
                "Left Leg"
            };
        }

        return part_names;
    }

    void player_body_parts::update( const rbx::model_instance& character ) noexcept {
        if ( !character.get_address() )
            return;

        const auto child_entries = character.get_children_entries<rbx::part>();

        if ( child_entries.empty() )
            return;

        const auto part_names = get_part_names();

        std::vector<rbx::child_entry<rbx::part>> temp_entries;
        temp_entries.reserve( part_names.size() );

        for ( const auto& entry : child_entries ) {
            if ( std::find( part_names.begin(), part_names.end(), entry.name ) != part_names.end() )
                temp_entries.push_back( entry );
        }

        rbx::utils::sync_lists( parts, temp_entries );
    }
}