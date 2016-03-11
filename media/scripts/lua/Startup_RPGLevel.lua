-- This method is called during first loading of a level in RPG
function Startup_RPGLevel()
	insertItemAtWaypoint("UsableItem", "WP_Item_01")
	insertItemAtWaypoint("UsableItem", "WP_Item_02")
	insertItemAtWaypoint("UsableItem", "WP_Item_03")
	insertItemAtWaypoint("UsableItem", "WP_Item_04")

	insertNPCAtWaypoint("NPC01", "WP_001")
end
