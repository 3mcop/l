var $j = jQuery.noConflict();

function safelog(data) {if(console){console.log(data)}}
/** 
 * @param link
 * @return the jQuery selector to use in finding the checkboxes we are attempting to manipulate
 */
function find_matching_checkboxes(link) {
	var module = $j(link)
								.parent('td')
								.parent('tr')
								.children('td.module_name')
								.attr('id');
	safelog('module name to modify: '+module);
	var ctd = $j(link).parent('td');
	// get the index of the current column
	var col_index = $j('tr.'+module+' td').index(ctd);
	col_index++;
	return 'tr.'+module+' td:nth-child('+col_index+') input:checkbox';
}

$j(document).ready(function(){

  // get number of columns we will need to manipulate & add 
  // select / unselect options for each module header row
  var roles = $j('#permissions > thead > tr > th.checkbox');
  var role_count = roles.size();
  
  // take off the stupid colspan from the module header row, because we 
  // will be matching up to the other rows perfectly now
  $j('td.module').removeAttr('colspan');
  
  // needed to add in a defining class or id to the parent TR in 
  // order to have a reference to grab a proper index for the links
  // being clicked.
  $j('td.module').each(function(){
    var module_id = $j(this).attr('id');
    $j(this)
    	.parent('tr')
    	.addClass(module_id+' module_parent')
    	.attr('rel', module_id);
  });
  
  // cycle how many roles we have and insert that many columns worth of 
  // select / deselect options
  $j(roles).each(function(){
	  $j('td.module')
	   		.after('<td class="pselect"><a href="#" class="check">check all</a>&nbsp;/&nbsp;<a href="#" class="uncheck">uncheck all</a></td>');
	  });

  // give the new boxes the module class to preserve styling of the row
  $j('td.pselect').addClass('module');

  // clicky 
  $j('a.check').click(function(){
	var check_it = find_matching_checkboxes(this);
	$j(check_it).attr('checked', true);
	return false;	
  });
  $j('a.uncheck').click(function(){
	var uncheck_it = find_matching_checkboxes(this);
	$j(uncheck_it).attr('checked', false);
	return false;	
  });  
});
