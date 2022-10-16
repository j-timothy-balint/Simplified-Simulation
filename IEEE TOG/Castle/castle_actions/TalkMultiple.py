
def applicability_condition(self,Agent,Subject,Subject2,Place):
	return SUCCESS

def preparatory_spec(self,Agent,Subject,Subject2,Place):
        if not contain(Place,Agent):
                changeContents(Place,Agent)
        if not contain(Place,Subject):
                changeContents(Place,Subject)
        if not contain(Place,Subject2):
                changeContents(Place,Subject2)
	useObjects(self.id,(Agent,Subject,Subject2))
	return SUCCESS

def execution_steps(self,Agent,Subject,Subject2,Place):
	return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent,Subject,Subject2,Place):
	if finishedAction(self.id):
		return SUCCESS
	return INCOMPLETE

