package org.renthusiasts.rcpp.client;

import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.CheckBox;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.Hyperlink;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.SimplePanel;
import com.google.gwt.user.client.ui.StackLayoutPanel;
import com.google.gwt.user.client.ui.Tree;
import com.google.gwt.user.client.ui.TreeItem;
import com.google.gwt.user.client.ui.Widget;

public class Rcpp extends Composite {

	private static RcppUiBinder uiBinder = GWT.create(RcppUiBinder.class);

	interface RcppUiBinder extends UiBinder<Widget, Rcpp> {
	}

	@UiField StackLayoutPanel stacks ;
	@UiField Tree vectors_tree ;
	@UiField SimplePanel content ;

	public Rcpp() {
		initWidget(uiBinder.createAndBindUi(this));
		stacks.showWidget(0) ;

		loadVectorTree( ) ;

	}

	void loadVectorTree(){
		String[] vectors = { "integer", "numeric", "logical", "raw", "character", "generic", "expression" } ; 
		for( String vec: vectors){
			vectors_tree.addItem( new VectorLabel( vec ) ) ;
		}
	}
	
	class VectorLabel extends HTML implements ClickHandler {
		
		public String type ;
		
		public VectorLabel(String type ){
			super( "<b>" + type + " vectors </b>" ) ;
			this.type = type ;
			setStyleName( "section" ) ;
			addClickHandler( this ) ;
		}

		@Override
		public void onClick(ClickEvent event) {
			content.clear();
			content.add( new HTML( "documentation of <b>" + type + "</b> vectors" )) ;
		}
		
	}
	
}
