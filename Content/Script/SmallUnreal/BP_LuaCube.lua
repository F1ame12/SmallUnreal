--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_LuaCube_C
local M = UnLua.Class()

function M:Initialize(Initializer)

end

function M:GenSomeStr()
    return "hhhhhhhhhh cube is nil"
end
function M:UserConstructionScript()
    --M.NewComp = self:AddComponentByClass(UE.UClass:GetClass("Engine/"), false, self.Cube:GetTransform(), false)
    --M.NewComp:K2_AttachTo(self.Cube)
end

function M:ReceiveBeginPlay()
    local SelfActor = self.Cube:GetOwner()
    local DebugStr = self:GenSomeStr()
    if (SelfActor) then
        DebugStr = "Hi im ready to do something!"
    end
    UE.UKismetSystemLibrary.PrintString(nil, DebugStr)
end

-- function M:ReceiveEndPlay()
-- end

-- function M:ReceiveTick(DeltaSeconds)
-- end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

return M
