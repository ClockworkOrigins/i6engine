-- This method is called during first loading of a level in RPG
function Startup()
	insertPlayerAtWaypoint("Player01", "Start")

	insertNPCAtWaypoint("NPC01", "WP_001")
end
