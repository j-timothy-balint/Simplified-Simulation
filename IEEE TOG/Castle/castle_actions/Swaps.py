
def applicability_condition(self,Agent,Object1,Object2,Place):
	return SUCCESS

def preparatory_spec(self,Agent,Object1,Object2,Place):
        if not contain(Place,Agent):
                changeContents(Place,Agent)
        if not contain(Place,Object1):
                changeContents(Place,Object1)
        if not contain(Place,Object2):
                changeContents(Place,Object2)
        useObjects(self.id,(Agent,Object1,Object2))
	return SUCCESS

def execution_steps(self,Agent,Object1,Object2,Place):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,Object1,Object2,Place):
	if finishedAction(self.id):
		return SUCCESS
	return INCOMPLETE

