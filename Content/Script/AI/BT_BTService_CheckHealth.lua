
local M = UnLua.Class()

function M:ReceiveTickAI(OwnerController, ControlledPawn, DeltaSeconds)
    
    local NowHealth = ControlledPawn.AttrComp:GetHealth()

    local CheckHealthVal = self.CheckHealthVal
    --UE.UKismetSystemLibrary.PrintString(ControlledPawn, "ai's check health is " .. tostring(CheckHealthVal))

    if OwnerController and OwnerController.Blackboard then
        if NowHealth <= CheckHealthVal then

            OwnerController.Blackboard:SetValueAsBool("IsLowHealth", true)

            --UE.UKismetSystemLibrary.PrintString(ControlledPawn, "ai's health is low than " .. tostring(CheckHealthVal) .. "!")
            
        else
            OwnerController.Blackboard:SetValueAsBool("IsLowHealth", false)

            --UE.UKismetSystemLibrary.PrintString(ControlledPawn, "ai's health is not low than " .. tostring(CheckHealthVal) .. "!")
        end
    end

    --UE.UKismetSystemLibrary.PrintString(ControlledPawn, "ai health = " .. tostring(NowHealth))

     
end

return M