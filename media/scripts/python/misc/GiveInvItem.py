# gives an item from one NPC to another one
def giveInvItems(self, other, item, amount):
	self.removeItems(item, amount)
	other.createItems(item, amount)
	return
