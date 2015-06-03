-- prints a message
function printMessage(name, x, y, message, alignment, lifetime, font)
	addPrint(name, "RPG/Blanko", x, y, message, alignment, lifetime)
	setSize(name, 1.0, 0.05)
	setFont(name, font)
end