<?php

// -----------------------------------
// Pagination - Functions

class Pagination {

    static function limitQuery($LIMIT=0)
	{
		if($LIMIT == 0) $LIMIT = PAGINATION_LIMIT;

        if(!isset($_GET['page'])) $Page = 1;
        else $Page = (int)$_GET['page'];

        $Start = ($Page * $LIMIT) - $LIMIT;
    
		return " LIMIT ".$Start.",".$LIMIT;
	}


	static function printMenu($num, $values="", $LIMIT=0)
	{
		if($LIMIT == 0) $LIMIT = PAGINATION_LIMIT;

        if(!isset($_GET['page'])) 
            $Page = 1;
        else 
            $Page = (int)$_GET['page'];
	    
	    if($num >= ($Page * $LIMIT))
	    {
	        $before = ($Page - 1);
	        $after = ($Page + 1);
	    }
	    else
	    {
	        $after = $Page;
	        $before = ($Page - 1);	
	    }
    
    	$MaxPage = (int)(($num / $LIMIT));

    	if($num % $LIMIT != 0) 
            $MaxPage++;

		if($before<=0) $addclass_pre = 'disabled';
		else $addclass_pre = ""; 

		if($Page==$MaxPage) $addclass_aft = 'disabled';
		else $addclass_aft = ""; 

		if($num !=0 && $Page > $MaxPage) goToLocation("?page=1".$values);


			if($Page > 5) $start = $Page-5;
			else $start = 1; 

			if($MaxPage < $Page+5) $end = $MaxPage;
			else $end = $Page+5;  			

		    $Pagination = " <nav>
 							 <ul class='pagination justify-content-center'>
							    <li class='page-item $addclass_pre'>
							      <a class='page-link' href='?page=1".$values."'>
							        <span>&laquo;</span>
							      </a>
							    </li>
							    <li class='page-item $addclass_pre'>
							      <a class='page-link' href='?page=".$before.$values."'>
							        <span>&lsaquo; Previous</span>
							      </a>
							    </li>";

			for($i=$start; $i<=$end; $i++)
			{
				if($i == $Page) $readd = "active";
				else $readd = "";
				$Pagination .= "<li class='page-item $readd'><a class='page-link' href='?page=".$i.$values."'>$i</a></li>";	
			}

			$Pagination .= "    <li class='page-item $addclass_aft'>
								    <a class='page-link' href='?page=".$after.$values."'>
								      <span>Next &rsaquo;</span>
								    </a>
								</li>
								<li class='page-item $addclass_aft'>
								    <a class='page-link' href='?page=".$MaxPage.$values."'>
								      <span>&raquo;</span>
								    </a>
								</li>";

			$Pagination .= "  </ul>
							</nav>";

		

		return $Pagination;
	}	

}