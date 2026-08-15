#pragma once

#include "instance.h"
#include "part.h"
#include "model_instance.h"
#include "humanoid.h"

namespace rbx {
	struct player_body_parts {
	private:
		std::vector<rbx::child_entry<rbx::part>> parts;

		std::vector<std::string> get_part_names() const noexcept;

	public:
		bool has( std::string name ) const noexcept {
			auto it = std::find_if( parts.begin(), parts.end(), [ &name ]( const rbx::child_entry<rbx::part>& p ) {
				return p.name == name;
				});
			return it != parts.end();
		}

		rbx::part get( std::string name ) const noexcept {
			auto it = std::find_if( parts.begin(), parts.end(), [ &name ]( const rbx::child_entry<rbx::part>& p ) {
				return p.name == name;
				} );
			return ( it != parts.end() && ( *it ).instance.is_valid() ) ? ( *it ).instance : rbx::part();
		}

		bool is_valid( void ) const noexcept {
			return has( "Head" ) && has( "HumanoidRootPart" ) && ( has( "UpperTorso" ) || has( "Torso" ) );
		}
		
		void update( const rbx::model_instance& character ) noexcept;
	};

	class player : public rbx::instance {
	public:
		// Static information, not frequently updated
		std::string username;
		bool is_teammate;

		// Dynamic info/data, updates frequently
		rbx::model_instance character;
		rbx::humanoid humanoid;
		float distance;
		float health;
		float max_health;
		rbx::instance equipped_tool;
		rbx::player_body_parts body_parts;

		// Functions
		rbx::instance get_team( void ) const noexcept;
		rbx::model_instance get_model_instance( void ) const noexcept;

		bool is_valid( void ) const noexcept {
			return get_address() != 0 && character.get_address() != 0 && humanoid.get_address() != 0 && body_parts.is_valid();
		}

		bool is_alive( void ) const noexcept {
			return humanoid.get_address() != 0 && humanoid.get_health() > 0;
		}
	};
}

namespace std {
	template<>
	struct hash<rbx::player> {
		size_t operator()( const rbx::player& player ) const {
			return std::hash<std::uintptr_t>()( player.address );
		}
	};

	template<>
	struct hash<rbx::part> {
		size_t operator()( const rbx::part& part ) const noexcept {
			return std::hash<std::uintptr_t>()( part.address );
		}
	};

	template<typename T>
	struct hash<rbx::child_entry<T>> {
		size_t operator()( const rbx::child_entry<T>& entry ) const noexcept {
			return std::hash<std::uintptr_t>()( entry.instance.address );
		}
	};
}