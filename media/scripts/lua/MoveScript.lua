function tick(id)
	local go = getObject(id)
	if (go ~= nil) then
		local c = go:getGOC(ComponentTypes.PhysicalStateComponent)
		if (c ~= nil) then
			local psc = getPhysicalStateComponent(c)
			if (psc ~= nil) then
				psc:applyCentralForce(Vec3(0.0, 0.0, -50.0), true)
			end
		end
	end
end

function shatter(selfID, otherID)
	local go = getObject(selfID)
	if (go ~= nil) then
		local c = go:getGOC(ComponentTypes.PhysicalStateComponent)
		if (c ~= nil) then
			local psc = getPhysicalStateComponent(c)
			if (psc ~= nil) then
				psc:applyCentralForce(Vec3(0.0, 1000.0, 5000.0), true)
				addPrint("ScriptCollision_" .. tostring(selfID), "RPG/Blanko", 0.5, 0.48, "A crash? Registered in a lua script? Nice!", Alignment.Center, 2000000)
				setSize("ScriptCollision_" .. tostring(selfID), 1.0, 0.3)
			end
		end
	end
end
