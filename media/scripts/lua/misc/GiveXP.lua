-- gives xp to an NPC
function giveXP(self, xp)
	self:setCurrentXP(self:getCurrentXP() + xp)
	if (self:getCurrentXP() >= self:getNextXP()) then
		self:setLevel(self:getLevel() + 1)
		self:setNextXP(self:getNextXP() + (self:getLevel() + 1) * 500)
	end
end
