function init(id)
	local go = getObject(id)
	if (go ~= nil) then
		local vc = go:getVelocityComponent()
		if (vc ~= nil) then
			vc:accelerate("")
		end
	end
end

function tick(id)
end

function shatter(selfID, otherID)
	local go = getObject(selfID)
	if (go ~= nil) then
		local psc = go:getPhysicalStateComponent()
		if (psc ~= nil) then
			psc:applyCentralForce(Vec3(0.0, 1000.0, 5000.0), true)
			addPrint("ScriptCollision_" .. tostring(selfID), "RPG/Blanko", 0.5, 0.48, "A crash? Registered in a lua script? Nice!", Alignment.Center, 3000000)
			setSize("ScriptCollision_" .. tostring(selfID), 1.0, 0.3)
		end
	end
end
