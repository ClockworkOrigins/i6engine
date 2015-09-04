-- gives xp to an NPC
function giveXP(self, xp)
	printMessage("XPMessage", 0.5, 0.45, getTextManager():getText("XPGained") .. tostring(xp), Alignment.Center, 5000000, "DejaVuSans-8")
	self:setCurrentXP(self:getCurrentXP() + xp)
	if (self:getCurrentXP() >= self:getNextXP()) then
		self:setLevel(self:getLevel() + 1)
		self:setNextXP(self:getNextXP() + (self:getLevel() + 1) * 500)
		printMessage("LevelupMessage", 0.5, 0.5, getTextManager():getText("Levelup"), Alignment.Center, 5000000, "DejaVuSansBig")
	end
end
