
def applicability_condition(self,Agent,Agent2,Object,Place):
	return SUCCESS

#Combined has and on here into one single contains
def preparatory_spec(self,Agent,Agent2,Object,Place):
        if not contain(Place,Agent):
                changeContents(Place,Agent)
        if not contain(Place,Agent2):
                changeContents(Place,Agent2)
        if not contain(Agent,Object):
                changeContents(Agent,Object)
	useObjects(self.id,(Agent,Agent2,Object))
	return SUCCESS

def execution_steps(self,Agent,Agent2,Object,Place):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,Agent2,Object,Place):
	if finishedAction(self.id):
		changeContentsMark(self.id,Agent2,Object)
		return SUCCESS
	return INCOMPLETE

