-- gives an item from one NPC to another one
function giveInvItems(self, other, item, amount)
	self:removeItems(item, amount)
	other:createItems(item, amount)
end
