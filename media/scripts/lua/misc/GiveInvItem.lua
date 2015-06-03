-- gives an item from one NPC to another one
function giveInvItems(self, other, item, amount)
	self:removeItems(item, amount)
	other:createItems(item, amount)
	if (self:getGO():getID() == getNPC("Player01"):getGO():getID()) then
		if (amount == 1) then
			printMessage("GiveItemsMessage", 0.5, 0.4, getItemName(item) .. getTextManager():getText("GaveItem"), Alignment.Center, 5000000, "DejaVuSans-8")
		else
			printMessage("GiveItemsMessage", 0.5, 0.4, tostring(amount) .. "x " .. getItemName(item) .. getTextManager():getText("GaveItem"), Alignment.Center, 5000000, "DejaVuSans-8")
		end
	else
		if (amount == 1) then
			printMessage("GiveItemsMessage", 0.5, 0.4, getItemName(item) .. getTextManager():getText("GotItem"), Alignment.Center, 5000000, "DejaVuSans-8")
		else
			printMessage("GiveItemsMessage", 0.5, 0.4, tostring(amount) .. "x " .. getItemName(item) .. getTextManager():getText("GotItem"), Alignment.Center, 5000000, "DejaVuSans-8")
		end
	end
end
