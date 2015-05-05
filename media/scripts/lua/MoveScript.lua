--package.loadlib('ScriptingObjectLua','init')()
package.loadlib('ScriptingMathLua','init')()

a = i6eVector(1.0, 2.0, 3.0)
print(a)

--function tick(id)
	--print(a:normalize())
	--print("tick called by id = " .. tostring(id))
	--print("tick PSC = " .. tostring(ComponentTypes.PhysicalStateComponent))
	--local go = getObject(id)
	--print(type(id))
	--print(type(go))
	--print(getmetatable(go))
	--if (go ~= nil) then
		--print("True")
		--go:setDie()
	--else
		--print("False")
	--end
--end

--function shatter(selfID, otherID)
--end
