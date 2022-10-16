
def applicability_condition(self,Agent,Object1,Object2,Place):
	return SUCCESS

#I changed the prepitory specs of this action because it didn't make much
#sense to have the object that should be a part of the agent to be
#in the location. Although it might not effect it too much...
def preparatory_spec(self,Agent,Object1,Object2,Place):
        if not contain(Place,Agent):
                changeContents(Place,Agent)
        if not contain(Place,Object2):
                changeContents(Place,Object2)
        if not contain(Agent,Object1):
                changeContents(Agent,Object1)
	useObjects(self.id,(Agent,Object1,Object2))
	return SUCCESS

def execution_steps(self,Agent,Object1,Object2,Place):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,Object1,Object2,Place):
	if finishedAction(self.id):
		changeContentsMark(self.id,Object2,Object1)
		return SUCCESS
	return INCOMPLETE

